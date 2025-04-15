import React from 'react'
import "./card.css"
const card = (props) => {
    return (
        <div className='Container'>
            <h3> Name :{props.name}</h3>
            <h5>Email :{props.email}</h5>
        </div>
    )
}

export default card