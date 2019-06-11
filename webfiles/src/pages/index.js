import React, {Component} from 'react';

export default class Main extends Component {
    constructor(props) {
        super(props);
        this.state = {
            availableWifi : []
        };
    }

    componentDidMount() {
        fetch('') //Should receive from esp. What should the URL be?
        .then(response => response.json())
        .then(data => this.setState({ availableWifi : data }));
    }

    render() {
        return (
            <form method="POST" encType="multipart/form-data">
                <input type="file" name="data"/>
            </form>
        );
    }
}